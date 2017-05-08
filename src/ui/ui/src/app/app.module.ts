import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { HttpModule } from '@angular/http';
import { FormsModule }   from '@angular/forms';
import { RouterModule } from '@angular/router';
import { MaterialModule } from '@angular/material';
import { NgbModule } from '@ng-bootstrap/ng-bootstrap';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';

import { AppRoot } from './root';
import { ROUTES } from './app.routes';
import { Home } from './components/home/home';
import { Login } from './components/login/login';
import { MasonryModule } from 'angular2-masonry';
import { LoginCallback } from "app/components/login/login-callback";
import { ChannelComponent } from './components/channel-block/channel-block';
import { PageSwitcherComponent } from './components/page-switcher/page-switcher';
import { QuickNavComponent } from './components/quick-nav/quick-nav';
import { MobileControlComponent } from './components/mobile-control/mobile-control';
import { Player3Client } from "app/services/player3-client/player3-client";
import { TwitchAPI } from "app/services/twitch/twitch";
import { WebSocketClient } from "app/services/player3-client/websocket-client";

@NgModule({
  declarations: [
    Home,
    Login,
    LoginCallback,
    AppRoot,
    ChannelComponent,
    PageSwitcherComponent,
    QuickNavComponent,
    MobileControlComponent,
  ],
  imports: [
    BrowserModule,
    HttpModule,
    FormsModule,
    MasonryModule,
    NgbModule.forRoot(),
    MaterialModule.forRoot(),
    RouterModule.forRoot(ROUTES),
    BrowserAnimationsModule
  ],
  providers: [ WebSocketClient, Player3Client, TwitchAPI ],
  bootstrap: [AppRoot]
})
export class AppModule { }
