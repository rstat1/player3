import { Config } from 'app/config'
import { Injectable } from '@angular/core';
import { Http, RequestOptions, Headers, Response } from '@angular/http';
import { Observable } from 'rxjs/Observable';

import 'rxjs/add/operator/map';
import 'rxjs/add/operator/catch';
import 'rxjs/add/operator/toPromise';

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
	constructor(private http: Http) {}
	public getFollows(live: boolean) : Observable<FollowedStream[]> {
		let streamsURL: string = Config.TWITCH_API + Config.STREAMS_URL;
		let headers: Headers = new Headers({
			 "Client-Id": Config.CLIENT_ID,
			 "Authorization": "OAuth " + localStorage.getItem("twitch_token")
		});
		let options: RequestOptions = new RequestOptions({headers: headers});

		return this.http.get(streamsURL, options).map(this.getStreams);
	}
	public getPlaylistURL(channelName: string) {
		let channelsURL = `${Config.TWITCH_CHANNELS_URL}/${channelName}/access_token`;
		let usherURL = `${Config.TWITCH_USHER_URL}/${channelName}.m3u8`;
		let headers: Headers = new Headers({
			 "Client-Id": Config.CLIENT_ID
		});
		let params: URLSearchParams = new URLSearchParams("adblock=false&need_https=true&platform=web&player_type=site");
		let options: RequestOptions = new RequestOptions({headers: headers, params: params});
		this.http.get(channelsURL, options).map(res => res.json()).subscribe(token => {
			let params: URLSearchParams = new URLSearchParams();
			params.set("player", "web");
			params.set("token", token.token);
			params.set("sig", token.sig);
			params.set("allow_audio_only", "true");
			params.set("player_backed", "html5");
			params.set("p", Math.round(Math.random() * 1e7).toString());
			options = new RequestOptions({params: params});
			this.http.get(usherURL, options).map(resp => resp.json()).subscribe(playlist => {
				console.log(playlist);
			})
		});
	}
	private getStreams(data: Response) {
		let followed = new Array<FollowedStream>();
		let streamBlob = data.json();
		let streams = streamBlob.streams;

		console.log(streamBlob);

		for (var index = 0; index < streamBlob._total; index++) {
			var element = streams[index];
			followed.push(new FollowedStream(element.game, Number(element.viewers),
											 element.preview.medium, element.channel.display_name,
											 element.channel.logo, element.channel.status));
		}
		return followed;
	}
}