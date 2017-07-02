import { Injectable, OnInit } from '@angular/core'
import { Player3Client } from 'app/services/player3-client/player3-client'

export class AppControl {
	public controlName: string;
	public controlText: string;
	public icon: string;
	constructor(public name: string, public text: string, iconName: string) {
		this.controlName = name;
		this.controlText = text;
		this.icon = iconName;
	}
}

@Injectable()
export class AppControls {
	private client: Player3Client;
	public isPlaying: boolean = false;
	public static appControls: AppControl[] = [];

	constructor(private p3Client: Player3Client) {
		this.client = p3Client;
		AppControls.appControls.push(new AppControl("stop", "Stop", "stop"));
		AppControls.appControls.push(new AppControl("mute", this.client.MuteButtonText, "volume-off"));
		AppControls.appControls.push(new AppControl("exit", "Exit", "stop"));
		AppControls.appControls.push(new AppControl("showChat", "Chat", "comment"));
		this.client.IsPlaying.subscribe(message => {
			this.isPlaying = message;
		})
	}
	public action(controlName: string): void {
		switch (controlName) {
			case "stop":
				this.client.StopStream();
				break;
			case "mute":
				this.client.Mute();
				break;
			case "exit":
				this.client.Exit();
				break;
			case "showChat":
				break;
		}
	}
}