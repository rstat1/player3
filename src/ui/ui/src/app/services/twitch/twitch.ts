import { Config } from 'app/config'
import { Injectable } from '@angular/core';
import { Http, RequestOptions, Headers, Response } from '@angular/http';
import { Observable } from 'rxjs/Observable';

import 'rxjs/add/operator/map';
import 'rxjs/add/operator/catch';
import 'rxjs/add/operator/toPromise';

export class FollowedStream {
	public Game: string;
	public Viewers: number;
	public PreviewImg: string;
	public ChannelName: string;
	public LogoURL: string;
	constructor(Game: string, ViewerCount: number, Preview: string, Name: string, Logo: string) {
		this.Game = Game;
		this.Viewers = ViewerCount;
		this.PreviewImg = Preview;
		this.ChannelName = Name;
		this.LogoURL = Logo;
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
	private getStreams(data: Response) {
		let followed = new Array<FollowedStream>();
		let streamBlob = data.json();
		let streams = streamBlob.streams;
		for (var index = 0; index < streamBlob._total; index++) {
			var element = streams[index];
			followed.push(new FollowedStream(element.game, Number(element.viewers),
											 element.preview.medium, element.channel.name,
											 element.channel.logo));
		}
		return followed;
	}
}