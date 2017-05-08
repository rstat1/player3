import { Component, OnInit } from '@angular/core';
import { Player3Client } from '../../services/player3-client/player3-client'

@Component({
	selector: 'quick-nav',
	providers: [],
  	templateUrl: './html/quick-nav.html',
  	styleUrls: ['./css/quick-nav.scss']
})
export class QuickNavComponent {
	public playing: boolean = false;

  	constructor(public client: Player3Client) {
		this.client.IsPlaying.subscribe(message => {
			this.playing = message;
		})
	}
}