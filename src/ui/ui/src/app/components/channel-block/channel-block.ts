import { Component, Input, HostListener } from '@angular/core';
import { trigger, state, style, animate, transition } from '@angular/animations';
import { Player3Client } from '../../services/player3-client/player3-client'

@Component({
	selector: 'channel',
	providers: [Player3Client],
	templateUrl:'./html/channel.html',
	styleUrls: ['./css/channel.scss'],
	animations: [
		trigger('imageHover', [
			state('mouseexit', style({
				opacity: 0.4,
			})),
			state('mouseenter', style({
				opacity: 1,
			})),
			transition('mouseexit => mouseenter', animate('100ms ease-in')),
			transition('mouseenter => mouseexit', animate('100ms ease-out'))
		]),
		trigger('topHover', [
			state('mouseenter', style({
				transform: 'translateY(0%)',
			})),
			state('mouseexit', style({
				transform: 'translateY(-20%)',
				display: 'none'
			})),
			transition('mouseexit => mouseenter', animate('100ms ease-in')),
			transition('mouseenter => mouseexit', animate('100ms ease-out'))
		]),
		trigger('bottomHover', [
			state('mouseenter', style({
				transform: 'translateY(0%)'
			})),
			state('mouseexit', style({
				transform: 'translateY(20%)',
				display: 'none'
			})),
			transition('mouseexit => mouseenter', animate('100ms ease-in')),
			transition('mouseenter => mouseexit', animate('100ms ease-out'))
		])
	]
})
export class ChannelComponent {
	@Input() channel: string;
	@Input() logoURL: string;
	@Input() viewers: string;
	@Input() streamTitle: string;

	public state = "mouseexit";

	constructor(private client: Player3Client) {}

	@HostListener('mouseenter') MouseEnter() {
		this.state = 'mouseenter';
	}
	@HostListener('mouseleave') MouseLeave() {
		this.state = 'mouseexit';
	}
	public toggleState() {
		this.state = (this.state === 'mouseenter' ? 'mouseleave' : 'mouseenter');
	}
	public startStream(channelName) {
		console.log(channelName);
		this.client.StartStream(channelName);
	}
}