[%bs.raw {|require('./app.css')|}];

module RouterConfig = {
  type route =
    | Home
    | Edit
    | List
    | Login;
  let routeFromUrl = (url: ReasonReact.Router.url) =>
    switch (url.path) {
    | ["edit"] => Edit
    | ["list"] => List
    | ["login"] => Login
    | _ => Home
    };
  let routeToUrl = (route: route) =>
    switch (route) {
    | Home => "/"
    | Edit => "/edit"
    | List => "/list"
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
        <h2> (ReasonReact.string("Workhours")) </h2>
      </div>
      <Router.Container>
        ...(
             (~currentRoute) =>
               <div>
                 <ul className="navigation">
                   <li>
                     <Router.Link route=Home>
                       (ReasonReact.string("Home"))
                     </Router.Link>
                   </li>
                   <li>
                     <Router.Link route=Edit>
                       (ReasonReact.string("Edit"))
                     </Router.Link>
                   </li>
                   <li>
                     <Router.Link route=List>
                       (ReasonReact.string("List"))
                     </Router.Link>
                   </li>
                 </ul>
                 (
                   switch (currentRoute) {
                   | RouterConfig.Home => <Home />
                   | RouterConfig.Edit => <NewWork />
                   | RouterConfig.List => <WorkList />
                   | RouterConfig.Login => <Login />
                   }
                 )
               </div>
           )
      </Router.Container>
    </div>,
};