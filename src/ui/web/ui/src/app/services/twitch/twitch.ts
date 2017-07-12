import { Config } from 'app/config'
import { Injectable } from '@angular/core';
import { Observable } from 'rxjs/Observable';
import { WebSocketService } from 'angular2-websocket-service'
import { Http, RequestOptions, Headers, Response } from '@angular/http';
import { WebSocketClient } from 'app/services/player3-client/websocket-client'

import 'rxjs/add/operator/map';
import 'rxjs/add/operator/catch';
import 'rxjs/add/operator/toPromise';

import * as querystring from "querystring";

export class FollowedStream {
	public Game: string;
	public Viewers: string;
	public PreviewImg: string;
	public ChannelName: string;
	public LogoURL: string;
	public CurrentTitle: string;
	constructor(Game: string, ViewerCount: number, Preview: string, Name: string, Logo: string, Title: string) {
		this.Game = Game;
		this.Viewers = ViewerCount.toLocaleString();
		this.PreviewImg = Preview;
		this.ChannelName = Name;
		this.LogoURL = Logo;
		this.CurrentTitle = Title;
	}
}

@Injectable()
export class TwitchAPI {
	constructor(private http: Http, private ws: WebSocketClient) {}
	public getFollows(live: boolean) : Observable<FollowedStream[]> {
		let streamsURL: string = Config.TWITCH_API + Config.STREAMS_URL;
		let headers: Headers = new Headers({
			 "Client-Id": Config.CLIENT_ID,
			 "Authorization": "OAuth " + localStorage.getItem("twitch_token")
		});
		let options: RequestOptions = new RequestOptions({headers: headers});

		return this.http.get(streamsURL, options).map(this.getStreams);
	}
	public startStream(channelName: string) {
		let usherURL: string;
		let accessTokenMessage: string;
		let playlistURL: string;
		let tokenParams = {
			"adblock": "false",
			"need_https": "true",
			"platform": "web",
			"player_type": "site"
		};
		accessTokenMessage = `${Config.CHANNELS_URL}/${channelName}/access_token?${querystring.stringify(tokenParams)}`;

		this.ws.SubscribeToMessage("ACCESS", false, message => {
			let token = JSON.parse(message);
			let usherParams = {
				"token": token.token,
				"sig": token.sig,
				"allow_source": "true",
				"allow_spectre": "true",
				"baking_bread": "true",
				"allow_audio_only": "true",
				"player_backed": "html5",
				"p": Math.round(Math.random() * 1e7).toString()
			}
		 	usherURL = `${Config.USHER_URL}/${channelName.toLowerCase()}.m3u8?${querystring.stringify(usherParams)}`;
			this.ws.SendMessage("USHER", usherURL);
		})

		this.ws.SubscribeToMessage("USHER", false, message => {
			this.ws.SendMessage("START", this.parseURLList(message));
		});
		this.ws.SendMessage("ACCESS", accessTokenMessage);
	}
	private parseURLList(urllist: string) : string {
		let urls: string[] = urllist.split('\n');
		return urls[4];
	}
	private getStreams(data: Response) {
		let followed = new Array<FollowedStream>();
		let streamBlob = data.json();
		let streams = streamBlob.streams;

		for (var index = 0; index < streamBlob._total; index++) {
			var element = streams[index];
			followed.push(new FollowedStream(element.game, Number(element.viewers),
											 element.preview.medium, element.channel.display_name,
											 element.channel.logo, element.channel.status));
		}
		return followed;
	}
}