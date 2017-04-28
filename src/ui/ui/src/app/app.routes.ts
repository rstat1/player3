import { Routes } from '@angular/router'

import { AppRoot } from './root'
import { Home } from './components/home/home'
import { Login } from './components/login/login';

export const ROUTES: Routes = [
	{path: '', component: AppRoot},
	{path: 'login', component: Login},
	{path: 'home', component: Home}
];
