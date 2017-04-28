import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpModule } from '@angular/http';
import { RouterModule } from '@angular/router';

import { ROUTES } from './app.routes';
import { Home } from './components/home/home';
import { Login } from './components/login/login';
import { AppRoot } from './root';

@NgModule({
  declarations: [
    Home,
    Login,
    AppRoot,
  ],
  imports: [
    BrowserModule,
    FormsModule,
    HttpModule,
    RouterModule.forRoot(ROUTES)
  ],
  providers: [],
  bootstrap: [AppRoot]
})
export class AppModule { }
