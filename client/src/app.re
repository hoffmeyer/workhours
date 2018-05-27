[%bs.raw {|require('./app.css')|}];

module RouterConfig = {
  type route =
    | Home
    | Work
    | Login;
  let routeFromUrl = (url: ReasonReact.Router.url) =>
    switch (url.path) {
    | ["work"] => Work
    | ["login"] => Login
    | _ => Home
    };
  let routeToUrl = (route: route) =>
    switch (route) {
    | Home => "/"
    | Work => "/work"
    | Login => "/login"
    };
};

module Router = Routing.CreateRouter(RouterConfig);

let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self =>
    <div className="App">
      <div className="App-header">
        <h2> (ReasonReact.stringToElement("Workhours")) </h2>
      </div>
      <Router.Container>
        ...(
             (~currentRoute) =>
               <div>
                 <ul className="navigation">
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
                   switch (currentRoute) {
                   | RouterConfig.Home => <Home />
                   | RouterConfig.Work => <NewWork />
                   | RouterConfig.Login => <Login />
                   }
                 )
               </div>
           )
      </Router.Container>
    </div>,
};