import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpModule } from '@angular/http';
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
    FormsModule,
    HttpModule,
    MasonryModule,
    NgbModule.forRoot(),
    MaterialModule.forRoot(),
    RouterModule.forRoot(ROUTES),
    BrowserAnimationsModule
  ],
  providers: [],
  bootstrap: [AppRoot]
})
export class AppModule { }
