import { Injectable } from '@angular/core'
import { Observable } from 'rxjs/Observable'
import { Subject } from 'rxjs/Subject';

import { Config } from "app/config";

export class Subscription {
	public SingleMessage: boolean;
	public Handler: (message: string) => void;
	constructor(handler: (message: string) => void, oneOff: boolean) {
		this.Handler = handler;
		this.SingleMessage = oneOff;
	}
}

@Injectable()
export class WebSocketClient {
	public IsConnected: Observable<boolean>;
	private isConnected: Subject<boolean>;
	private socket: WebSocket;
	private ValidMessageTypes: Array<string> = [ "ACCESS", "USHER", "START", "STOP", "MUTE",
												 "ID", "PLAYERSTATE", "EXIT", "JOIN" ];
	private messageSubscriptions: Map<string, Subscription>;

	constructor() {
		if (this.socket == undefined)
		{
			this.isConnected = new Subject<boolean>();
			this.IsConnected = this.isConnected.asObservable();
			this.messageSubscriptions = new Map<string, Subscription>();
			this.SubscribeToMessage("ID", true, message => {
				//this.SendMessage("ID", Config.CLIENT_ID);
			});
			this.socket = new WebSocket("ws://" + Config.getP3ClientEndpoint());
			this.socket.onmessage = event => { this.OnMessageReceived(event); }
			this.socket.onopen = event =>
			{
				this.SendMessage("ID", Config.CLIENT_ID);
				this.isConnected.next(true);
			}
		}
	}
	public SubscribeToMessage(prefix: string, oneOff: boolean, handler: (message: string) => void) {
		if (this.ValidMessageTypes.includes(prefix) == true) {
			let sub: Subscription = new Subscription(handler, oneOff);
			this.messageSubscriptions[prefix] = sub;
		}
	}
	public SendMessage(messageType: string, args: string) {
		if (this.ValidMessageTypes.includes(messageType) == true) {
			this.socket.send(messageType + ":" + args);
		}
		else { console.error("Invalid Message: " + messageType); }
	}
	private OnMessageReceived(event: MessageEvent): any {
		let handler: Subscription;
		let data = event.data as string;
		let cmd = data.substring(0, data.indexOf(":") + 1).replace(":", "");
		let args = data.replace(cmd + ":", "");

		if (this.messageSubscriptions[cmd] !== undefined)
		{
			handler = this.messageSubscriptions[cmd];
			if (args !== "") { handler.Handler(args); }
		 	else { handler.Handler(null); }
		 	if (handler.SingleMessage) { this.messageSubscriptions.delete(args[0]); }
		}
		else { console.log("No handler found for " + cmd); }
	}
}