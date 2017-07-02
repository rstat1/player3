import { Injectable } from '@angular/core'
import { Observable } from 'rxjs/Observable'
import { Subject } from 'rxjs/Subject';
import { BehaviorSubject } from 'rxjs/BehaviorSubject';

import { Config } from 'app/config';
import { TwitchAPI } from 'app/services/twitch/twitch'
import { WebSocketClient } from 'app/services/player3-client/websocket-client'

@Injectable()
export class Player3Client {

	public IsPlaying: Observable<boolean>;
	public MuteButtonText: string = "Mute";
	public isMuted: boolean = false;
	private isPlaying: Subject<boolean>;
	private playingStream: boolean = false;

	constructor(private socket: WebSocketClient, private twitch: TwitchAPI) {
		this.isPlaying = new Subject<boolean>();
		this.IsPlaying = this.isPlaying.asObservable();
		this.socket.SubscribeToMessage("PLAYERSTATE", false, message => {
			let state: string[] = message.split(";");
			state[0] == "Muted" ? this.SetVolumeState(true) : this.SetVolumeState(false);
			state[1] == "Playing" ? this.SetPlayingState(true) : this.SetPlayingState(false);
		})
	}
	public StartStream(streamName: string) {
		if (this.playingStream) { this.StopStream(); }
		this.twitch.startStream(streamName);
	}
	public StopStream() {
		this.socket.SendMessage("STOP", "");
		this.SetVolumeState(false);
	}
	public Mute() {
		this.socket.SendMessage("MUTE", "");
	}
	public Exit() {
		this.socket.SendMessage("EXIT", "");
	}
	private SetVolumeState(muted: boolean) {
		this.isMuted = !this.isMuted;
		if (this.isMuted) { this.MuteButtonText = "Unmute"; }
		else { this.MuteButtonText = "Mute"; }
	}
	private SetPlayingState(isPlaying: boolean) {
		this.isPlaying.next(isPlaying);
		this.playingStream = isPlaying;
	}
}