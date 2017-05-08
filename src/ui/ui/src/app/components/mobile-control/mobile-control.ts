import { Component, OnInit } from '@angular/core';

import { AppRoutes } from '../../app.routes'
import { Player3Client } from '../../services/player3-client/player3-client'

@Component({
	selector: 'mobile-controls',
	providers: [],
	templateUrl: './html/mobile-control.html',
	styleUrls: ['./css/mobile-control.scss']
})
export class MobileControlComponent {
	public routes = AppRoutes.AppRoutes;
	public playing: boolean = false;

	constructor(public client: Player3Client) {
		this.client.IsPlaying.subscribe(message => {
			this.playing = message;
		})
	}
}