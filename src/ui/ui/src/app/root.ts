import { Router } from '@angular/router';
import { Component } from '@angular/core';

import { Config } from './config'
import { Auth } from './services/auth0/auth';

@Component({
	selector: 'app-root',
	providers: [Auth],
	templateUrl: './html/root.html',
	styleUrls: ['./css/root.css']
})
export class AppRoot {
	title2 = 'Welcome, player1';
  	constructor(private auth: Auth, private router: Router) {
		Config.setP3ClientEndpoint(window.location.hostname);
	}
}