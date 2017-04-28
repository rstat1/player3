import { Router } from '@angular/router';
import { Injectable } from '@angular/core';
import { tokenNotExpired } from 'angular2-jwt';
import { WebAuth, Auth0Error, Auth0DecodedHash } from "auth0-js";

@Injectable()
export class Auth {
	lock: WebAuth = new WebAuth({
		clientID: 'mmjl9gTqEntes22KKGAOfL3fXJch74uz',
		domain: 'rstat1.auth0.com',
	});
	constructor(private router: Router) {}
	public doAuth()	{
		this.lock.parseHash(null, (err, authResult) => {
			if (authResult && authResult.accessToken && authResult.idToken) {
				const expiresAt = JSON.stringify(
      				(authResult.expiresIn * 1000) + new Date().getTime()
    			);
				localStorage.setItem('access_token', authResult.accessToken);
				localStorage.setItem('id_token', authResult.idToken);
				localStorage.setItem('expires_at', expiresAt);
				this.router.navigate(["/home"]);
			}
		});
	}
	public login() {
		this.lock.authorize({
			connection: "twitch",
			responseType: 'token id_token',
			redirectUri: 'http://192.168.1.12:4200/home'
		})
	}
	// public logout() {
	// 	localStorage.removeItem('id_token');
	// }
	public authenticated() {
		return tokenNotExpired('id_token');
	}
}