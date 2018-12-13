open Types;

type routes =
  | Home
  | Edit
  | List
  | Login;

let urlToRoute = (url: ReasonReact.Router.url) =>
  switch (url.path) {
  | ["edit"] => Edit
  | ["list"] => List
  | ["login"] => Login
  | _ => Home
  };

let routeToString = (route: routes) =>
  switch (route) {
  | Home => "/"
  | Edit => "/edit"
  | List => "/list"
  | Login => "/login"
  };

let routeToComponent =
    (route: routes, ~workList: array(work), ~handleAction: action => unit) =>
  switch (route) {
  | Home => <Home workList handleAction />
  | Edit => <NewWork handleAction />
  | List => <WorkList workList handleAction />
  | Login => <Login />
  };