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

let routeToComponent = (route: routes, ~workList: array(work)) =>
  switch (route) {
  | Home => <Home workList />
  | Edit => <NewWork />
  | List => <WorkList workList />
  | Login => <Login />
  };