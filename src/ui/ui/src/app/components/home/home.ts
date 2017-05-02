import { Router } from '@angular/router';
import { DomSanitizer } from '@angular/platform-browser';
import { AngularMasonry, MasonryOptions } from 'angular2-masonry';
import { NgbModule, NgbCollapse } from '@ng-bootstrap/ng-bootstrap';
import { Component, ViewChild, OnInit, Pipe, PipeTransform, HostListener } from '@angular/core';
import { trigger, state, style, animate, transition } from '@angular/animations';

import { Auth } from "app/services/auth0/auth";
import { TwitchAPI, FollowedStream } from 'app/services/twitch/twitch';

@Pipe({name: 'SafeImgURL'})
export class SafeImgURLPipe implements PipeTransform {
	constructor(private sanitized: DomSanitizer) {}
	transform(value: any, ...args: any[]) {
		return this.sanitized.bypassSecurityTrustStyle(value);
	}
}

@Component({
	selector: 'app-home',
	providers: [ TwitchAPI ],
	templateUrl: './html/home.html',
	styleUrls: ['./styles/home.scss'],
})
export class Home implements OnInit {
	username = '';
	avatar_url = '';
	imgURL: SafeImgURLPipe;
	streams: FollowedStream[] = null;
	public isNavbarCollapsed = true;
	public sanitizer: DomSanitizer;

	@ViewChild(AngularMasonry) masonry: AngularMasonry;
	options: MasonryOptions = {
		gutter: 10,
		initLayout: true
	}

	constructor(private auth: Auth, private routes: Router, private twitch: TwitchAPI,
				private sanitized: DomSanitizer) {
		if (auth.authenticated() == false) { this.routes.navigate(['login']); }
		if (localStorage.getItem("twitch_avatar") != "none") {
			this.avatar_url = localStorage.getItem("twitch_avatar");
		}
		else { this.avatar_url = "/assets/user-default.png"; }
		this.username = localStorage.getItem("twitch_username");
		this.sanitizer = sanitized
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
}