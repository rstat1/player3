import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpModule } from '@angular/http';
import { RouterModule } from '@angular/router';
import { MaterialModule } from '@angular/material';
import { NgbModule } from '@ng-bootstrap/ng-bootstrap';

import { ROUTES } from './app.routes';
import { Home } from './components/home/home';
import { Login } from './components/login/login';
import { AppRoot } from './root';
import { LoginCallback } from "app/components/login/login-callback";
import { ChannelComponent } from './components/channel-block/channel-block'

@NgModule({
  declarations: [
    Home,
    Login,
    LoginCallback,
    AppRoot,
    ChannelComponent,
  ],
  imports: [
    BrowserModule,
    FormsModule,
    HttpModule,
    NgbModule.forRoot(),
    MaterialModule.forRoot(),
    RouterModule.forRoot(ROUTES),
  ],
  providers: [],
  bootstrap: [AppRoot]
})
export class AppModule { }
