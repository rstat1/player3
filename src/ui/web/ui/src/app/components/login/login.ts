import { Component } from '@angular/core'
import { Router } from '@angular/router'

import { Auth } from '../../services/auth0/auth';
import { MaterialImports } from '../../modules/material'
import { MdButtonModule } from "@angular/material";

@Component({
	selector: 'login',
	templateUrl: './html/login.html',
	styleUrls: ['./styles/login.scss']
})
export class Login {
	constructor(private loginButton: MdButtonModule, private auth: Auth, private routes: Router) {
		if (!this.auth.authenticated()) { this.auth.doAuth(); }
		else { this.routes.navigate(['home']); }
	}
	onLoginClick() {
		this.auth.login();
	}
}