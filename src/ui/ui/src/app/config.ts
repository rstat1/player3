import { environment } from 'app/environment'


export class Config {
	public static CLIENT_ID: string = "xaqx8q092rlsjzfh0sgruo90by2u13";
	public static STREAMS_URL: string = "/streams/followed";
	public static TWITCH_API: string = "https://api.twitch.tv/kraken";
	public static TWITCH_CHANNELS_URL: string = "https://api.twitch.tv/api/channels";
	public static TWITCH_USHER_URL: string = "https://usher.ttvnw.net/api/channels/hls";
	private static CLIENT_ENDPOINT: string = "";

	public static getP3ClientEndpoint() : string
	{
		if (environment.production == false) { return this.CLIENT_ENDPOINT + ":8080/"; }
		return this.CLIENT_ENDPOINT;
	}
	public static setP3ClientEndpoint(endpoint: string)
	{
		this.CLIENT_ENDPOINT = endpoint;
	}
	public static getAuth0Callback() : string { return environment.Auth0Callback; }
	public static getAuth0ClientID() : string { return environment.Auth0ClientID; }
	public static getAuth0Domain() : string { return environment.Auth0Domain; }
	public static isProduction() : boolean { return environment.production; }
}