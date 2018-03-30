[%bs.raw {|require('./app.css')|}];

[@bs.module] external logo : string = "./logo.svg";

module RouterConfig = {
  type route =
    | Work
    | Home;
  let routeFromUrl = (url: ReasonReact.Router.url) =>
    switch url.path {
    | ["work"] => Work
    | _ => Home
    };
  let routeToUrl = (route: route) =>
    switch route {
    | Work => "/work"
    | Home => "/"
    };
};

module Router = Routing.CreateRouter(RouterConfig);

let component = ReasonReact.statelessComponent("App");

let make = (~text, _children) => {
  ...component,
  render: (self) =>
    <div className="App">
      <div className="App-header">
        <img src=logo className="App-logo" alt="logo" />
        <h2> (ReasonReact.stringToElement(text)) </h2>
      </div>
      <Router.Container>
      ...(
           (~currentRoute) =>
             <div>
               <ul>
                 <li>
                 <Router.Link route=Home>
                   (ReasonReact.stringToElement("Home"))
                 </Router.Link>
                 </li>
                 <li>
                 <Router.Link route=Work>
                   (ReasonReact.stringToElement("Work"))
                 </Router.Link>
                 </li>
               </ul>
               (
                 switch currentRoute {
                 | RouterConfig.Home => <Home />
                 | RouterConfig.Work => <Work />
                 }
               )
             </div>
         )
</Router.Container>
    </div>
};
