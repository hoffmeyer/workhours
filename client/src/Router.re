module WithRouter = {
  type state = {currentRoute: Config.routes};
  type action =
    | ChangeUrl(Config.routes);
  let component = ReasonReact.reducerComponent("WithRouter");
  let make = children => {
    ...component,
    initialState: () => {
      currentRoute:
        ReasonReact.Router.dangerouslyGetInitialUrl() |> Config.urlToRoute,
    },
    reducer: (action, _state) =>
      switch (action) {
      | ChangeUrl(route) => ReasonReact.Update({currentRoute: route})
      },
    didMount: self => {
      let watchId =
        ReasonReact.Router.watchUrl(url =>
          self.send(ChangeUrl(url |> Config.urlToRoute))
        );
      self.onUnmount(() => ReasonReact.Router.unwatchUrl(watchId));
    },
    render: ({state}) => children(~currentRoute=state.currentRoute),
  };
};

module Link = {
  let component = ReasonReact.statelessComponent("Link");
  let make = (~route, ~className="", children) => {
    ...component,
    render: self => {
      let href = Config.routeToString(route);
      let onClick = handle =>
        handle((event, _self) => {
          event->ReactEvent.Mouse.preventDefault;
          ReasonReact.Router.push(href);
        });
      <a className onClick={onClick(self.handle)} href> ...children </a>;
    },
  };
};

module NavLink = {
  let component = ReasonReact.statelessComponent("NavLink");
  let make = (~route, ~activeRoute=?, ~className=?, children) => {
    ...component,
    render: _self =>
      <WithRouter>
        ...{
             (~currentRoute) => {
               let activeRoute =
                 switch (activeRoute) {
                 | None => route
                 | Some(route) => route
                 };
               let className =
                 Cn.make([
                   Cn.unpack(className),
                   Cn.ifTrue("active", activeRoute == currentRoute),
                 ]);
               <Link route className> ...children </Link>;
             }
           }
      </WithRouter>,
  };
};