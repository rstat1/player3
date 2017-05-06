import { Component, OnInit } from '@angular/core';
import { Player3Client } from '../../services/player3-client/player3-client'

@Component({
	selector: 'quick-nav',
	providers: [Player3Client],
  	templateUrl: './html/quick-nav.html',
  	styleUrls: ['./css/quick-nav.scss']
})
export class QuickNavComponent {
	public playing: boolean = false;

  	constructor(private client: Player3Client) {
		console.log("player state changed.")
		this.client.IsPlaying.subscribe(message => {
			this.playing = message;
		})
	}
}