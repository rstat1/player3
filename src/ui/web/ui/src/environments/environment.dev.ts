// The file contents for the current environment will overwrite these during build.
// The build system defaults to the dev environment which uses `environment.ts`, but if you do
// `ng build --env=prod` then `environment.prod.ts` will be used instead.
// The list of which env maps to which file can be found in `.angular-cli.json`.

export const environment = {
  production: false,
  Auth0Domain: "rstat1.auth0.com",
  Auth0Callback: "http://192.168.1.12:8080/login",
  Auth0ClientID: "mmjl9gTqEntes22KKGAOfL3fXJch74uz",
  P3ClientEndpoint: "ws://192.168.1.12:8080"
};
