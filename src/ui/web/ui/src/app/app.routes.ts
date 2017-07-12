import { Routes } from '@angular/router'

import { AppRoot } from './root'
import { Home } from './components/home/home'
import { Login } from './components/login/login';
import { LoginCallback } from "app/components/login/login-callback";

export const ROUTES: Routes = [
	{path: '', component: Home},
	{path: 'login', component: Login},
	{path: 'callback', component: LoginCallback},
	//{path: '**', redirectTo: ''}
];

export class AppRoutes {
	public static AppRoutes = [
		{itemName: 'Games', path: "directory/games"},
		{itemName: 'Discover', path: 'directory'},
		{itemName: 'Now Playing', path: ''},
	]
}