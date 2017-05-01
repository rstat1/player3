import { environment } from 'app/environment'


export class Config {
	public static CLIENT_ID: string = "xaqx8q092rlsjzfh0sgruo90by2u13";
	public static STREAMS_URL: string = "/streams/followed";
	public static TWITCH_API: string = "https://api.twitch.tv/kraken";

	public static getTwitchAPIEndpoint() : string { return environment.TwitchAPIEndpoint; }
	public static getAuth0Callback() : string { return environment.Auth0Callback; }
	public static getAuth0ClientID() : string { return environment.Auth0ClientID; }
	public static getAuth0Domain() : string { return environment.Auth0Domain; }
	public static isProduction() : boolean { return environment.production; }
}