import { Component } from '@angular/core';
import { Router } from '@angular/router';
import { MaterialImports } from './modules/material';

import { Auth } from './services/auth0/auth';

@Component({
	selector: 'app-root',
	providers: [Auth],
	templateUrl: './html/root.html',
	styleUrls: ['./css/root.css']
})
export class AppRoot {
	title = 'Welcome, player3';
  	constructor(private auth: Auth, private router: Router) {
		if(auth.authenticated() == false) {
			router.navigate(["login"]);
		}
    }
}