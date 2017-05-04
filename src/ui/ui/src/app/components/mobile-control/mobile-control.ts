import { Component, OnInit } from '@angular/core';

import { AppRoutes } from '../../app.routes'

@Component({
  selector: 'mobile-controls',
  templateUrl: './html/mobile-control.html',
  styleUrls: ['./css/mobile-control.scss']
})
export class MobileControlComponent {
  routes = AppRoutes.AppRoutes;
  
  constructor() { }
}
