import { Component, OnInit } from '@angular/core';

import { AppControl, AppControls } from 'app/services/app-control/app-control'
import { Player3Client } from 'app/services/player3-client/player3-client'

@Component({
	selector: 'quick-nav',
	providers: [],
  	templateUrl: './html/quick-nav.html',
  	styleUrls: ['./css/quick-nav.scss']
})
export class QuickNavComponent implements OnInit {
	controlsList: AppControl[];
  	constructor(public client: Player3Client, public controls: AppControls) {}
	ngOnInit(): void { this.controlsList = AppControls.appControls; }
}