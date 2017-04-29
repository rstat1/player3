import { Router } from '@angular/router';
import { Component } from '@angular/core';

import { Auth } from "app/services/auth0/auth";

@Component({
	selector: 'app-home',
	templateUrl: './html/home.html',
	styleUrls: ['./styles/home.css']
})
export class Home {
	title = "hello!";
	constructor(private auth: Auth, private routes: Router) {
		if (auth.authenticated()) { this.title = "hello world!" }
		else { this.routes.navigate(['login']); }
	}
}