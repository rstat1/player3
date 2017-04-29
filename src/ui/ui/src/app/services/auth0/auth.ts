import 'rxjs/add/operator/filter';
import { Config } from 'app/config'
import { Injectable } from '@angular/core';
import { tokenNotExpired } from 'angular2-jwt';
import { Router, NavigationStart } from '@angular/router';
import { WebAuth, Auth0Error, Auth0DecodedHash } from "auth0-js";

@Injectable()
export class Auth {
	lock: WebAuth = new WebAuth({
		clientID: Config.getAuth0ClientID(),
		domain: Config.getAuth0Domain(),
	});
	constructor(private router: Router) {
		console.log(Config.getAuth0Callback());
	}
	public doAuth()	{
		this.lock.parseHash({_idTokenVerification: false}, (err, authResult) => {
		 	if (err) { return console.log(err); }
			if (authResult && authResult.accessToken && authResult.idToken) {
				const expiresAt = JSON.stringify(
					(authResult.expiresIn * 1000) + new Date().getTime()
				);
				localStorage.setItem('access_token', authResult.accessToken);
				localStorage.setItem('id_token', authResult.idToken);
				localStorage.setItem('expires_at', expiresAt);
				this.lock.client.userInfo(authResult.accessToken, (err, result) => {
					if (!err)
					{
						let r: any = result;
						localStorage.setItem("twitch_username", r.name);
						localStorage.setItem("twitch_avatar", r.logo);
						localStorage.setItem("twitch_token", r.token);
					}
				});
				this.router.navigate(['/home']);
			}
		});
	}
	public login() {
		this.lock.authorize({
			connection: "twitch",
			responseType: 'token id_token',
			redirectUri: Config.getAuth0Callback()
		})
	}
	public logout() {
		localStorage.removeItem('id_token');
		localStorage.removeItem('expires_at');
		localStorage.removeItem('access_token');
		localStorage.removeItem('twitch_token');
		localStorage.removeItem('twitch_avatar');
		localStorage.removeItem('twitch_username');
	}
	public authenticated(): boolean {
		return localStorage.getItem("access_token") != null;
	}
}