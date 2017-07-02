import { Component, OnInit } from '@angular/core';

import { AppRoutes } from '../../app.routes'
import { Player3Client } from 'app/services/player3-client/player3-client'
import { AppControls, AppControl } from "app/services/app-control/app-control";

@Component({
	selector: 'mobile-controls',
	providers: [],
	templateUrl: './html/mobile-control.html',
	styleUrls: ['./css/mobile-control.scss']
})
export class MobileControlComponent implements OnInit {
	controlsList: AppControl[];
	public routes = AppRoutes.AppRoutes;
	constructor(public client: Player3Client, public controls: AppControls) {}
	ngOnInit(): void { this.controlsList = AppControls.appControls; }
}