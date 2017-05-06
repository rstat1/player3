import { Injectable } from '@angular/core'
import { QueueingSubject } from 'queueing-subject'
import { Observable } from 'rxjs/Observable'
import { Subject } from 'rxjs/Subject';
import { WebSocketService } from 'angular2-websocket-service'

import { Config } from 'app/config';
import { TwitchAPI } from 'app/services/twitch/twitch'

@Injectable()
export class Player3Client {
	public output: Observable<any>;
	public IsPlaying: Observable<boolean>;
	private isPlaying: Subject<boolean>;
	private input: QueueingSubject<any>;

	constructor(private socketService: WebSocketService, private twitch: TwitchAPI) {
		this.isPlaying = new Subject<boolean>();
		this.IsPlaying = this.isPlaying.asObservable();
	}
	public Connect(): Observable<any> {
		if (this.output) { return this.output; }
		else
		{
			this.input = new QueueingSubject<any>();
			this.output = this.socketService.connect('ws://' + Config.getP3ClientEndpoint(), this.input).share();
		}
	}
	public StartStream(streamName: string) {
		this.isPlaying.next(true);
		console.log(this.isPlaying.observers);
		//this.twitch.getPlaylistURL(streamName);
		//TODO: Convert streamName to stream url.
	}
	public StopStream() {
		this.isPlaying.next(false);
	}
	public Mute() {

	}
}