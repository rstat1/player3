import { Component } from '@angular/core';
import { Router } from '@angular/router';

import { Auth } from './services/auth0/auth';

@Component({
	selector: 'app-root',
	providers: [Auth],
	templateUrl: './html/root.html',
	styleUrls: ['./css/root.css']
})
export class AppRoot {
	title2 = 'Welcome, player1';
  	constructor(private auth: Auth, private router: Router) { }
}