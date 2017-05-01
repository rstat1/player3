import { Router } from '@angular/router';
import { Component } from '@angular/core';
import { NgbModule, NgbCollapse } from '@ng-bootstrap/ng-bootstrap';

import { Auth } from "app/services/auth0/auth";
import { TwitchAPI, FollowedStream } from 'app/services/twitch/twitch';

@Component({
	selector: 'app-home',
	providers: [ TwitchAPI ],
	templateUrl: './html/home.html',
	styleUrls: ['./styles/home.scss'],
})
export class Home {
	username = '';
	avatar_url = '';
	streams: FollowedStream[] = null;
	public isNavbarCollapsed = true;

	constructor(private auth: Auth, private routes: Router, private twitch: TwitchAPI) {
		if (auth.authenticated() == false) { this.routes.navigate(['login']); }
		if (localStorage.getItem("twitch_avatar") != "none") { this.avatar_url = localStorage.getItem("twitch_avatar"); }
		else { this.avatar_url = "/assets/user-default.png"; }
		this.username = localStorage.getItem("twitch_username");

		twitch.getFollows(true).subscribe(follows => this.streams);
	}
}