import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpModule } from '@angular/http';
import { RouterModule } from '@angular/router';
import { MaterialModule } from '@angular/material';

import { ROUTES } from './app.routes';
import { Home } from './components/home/home';
import { Login } from './components/login/login';
import { AppRoot } from './root';
import { LoginCallback } from "app/components/login/login-callback";

@NgModule({
  declarations: [
    Home,
    Login,
    LoginCallback,
    AppRoot,
  ],
  imports: [
    BrowserModule,
    FormsModule,
    HttpModule,
    RouterModule.forRoot(ROUTES),
    MaterialModule.forRoot()
  ],
  providers: [],
  bootstrap: [AppRoot]
})
export class AppModule { }
