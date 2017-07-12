import { Router } from '@angular/router';
import { DomSanitizer } from '@angular/platform-browser';
import { Component, ViewChild, OnInit } from '@angular/core';
import { AngularMasonry, MasonryOptions } from 'angular2-masonry';
import { trigger, state, style, animate, transition } from '@angular/animations';

import { Config } from "app/config";
import { Auth } from "app/services/auth0/auth";
import { WebSocketClient } from 'app/services/player3-client/websocket-client'
import { TwitchAPI, FollowedStream } from 'app/services/twitch/twitch';

@Component({
	selector: 'app-home',
	providers: [],
	templateUrl: './html/home.html',
	styleUrls: ['./styles/home.scss'],
})
export class Home implements OnInit {
	username = '';
	avatar_url = '';
	streams: FollowedStream[] = null;
	public isNavbarCollapsed = true;
	public sanitizer: DomSanitizer;

	@ViewChild(AngularMasonry) masonry: AngularMasonry;

	constructor(private auth: Auth, private routes: Router, private twitch: TwitchAPI,
				private sanitized: DomSanitizer, private ws: WebSocketClient) {
		if (auth.authenticated() == false) { this.routes.navigate(['login']); }
		if (localStorage.getItem("twitch_avatar") != "none") {
			this.avatar_url = localStorage.getItem("twitch_avatar");
		}
		else { this.avatar_url = "/assets/user-default.png"; }
		this.username = localStorage.getItem("twitch_username");
		this.sanitizer = sanitized
		this.ws.SubscribeToMessage("PLAYERSTATE", true, message => {
			
		});
	}
	ngOnInit(): void {
		this.twitch.getFollows(true).subscribe(follows => {
			this.streams = follows;
		});
	}
	public getURL(s): any {
		let style: string = "background-image: url('" + s + "');";
		this.sanitized.bypassSecurityTrustStyle(style);
		return 'url(' + s + ')';
	}
	public startStream(name) {
		console.log(name);
	}
}