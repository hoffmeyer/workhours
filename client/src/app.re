[%bs.raw {|require('./app.css')|}];

let component = ReasonReact.statelessComponent("App");

let make = (~currentRoute, _children) => {
  ...component,
  render: _self =>
    <div className="App">
      <div className="App-header">
        <h2> (ReasonReact.string("Workhours")) </h2>
      </div>
      <div>
        <nav>
          <ul className="navigation">
            <li>
              <Router.NavLink route=Config.Home>
                (ReasonReact.string("Home"))
              </Router.NavLink>
            </li>
            <li>
              <Router.NavLink route=Config.Edit>
                (ReasonReact.string("Edit"))
              </Router.NavLink>
            </li>
            <li>
              <Router.NavLink route=Config.List>
                (ReasonReact.string("List"))
              </Router.NavLink>
            </li>
          </ul>
        </nav>
        <main> (Config.routeToComponent(currentRoute)) </main>
      </div>
    </div>,
};