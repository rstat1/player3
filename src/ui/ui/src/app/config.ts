import { environment } from 'app/environment'

export class Config {
	public static getAuth0Callback() : string { return environment.Auth0Callback; }
	public static getAuth0ClientID() : string { return environment.Auth0ClientID; }
	public static getAuth0Domain() : string { return environment.Auth0Domain; }
	public static isProduction() : boolean { return environment.production; }
}