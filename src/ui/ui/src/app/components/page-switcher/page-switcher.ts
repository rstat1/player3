import { Component, OnInit } from '@angular/core';
import { NgbModule, NgbDropdown } from '@ng-bootstrap/ng-bootstrap';

import { AppRoutes } from '../../app.routes'

@Component({
  selector: 'nav-box',
  templateUrl: './html/page-switcher.html',
  styleUrls: ['./css/page-switcher.scss']
})
export class PageSwitcherComponent {
	routes = AppRoutes.AppRoutes;
	constructor() { }
	public nav(path) {
	}
}
