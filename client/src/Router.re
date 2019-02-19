type routes =
  | Home
  | Edit(option(string))
  | Login
  | NotFound;

let urlToRoute = (url: ReasonReact.Router.url) =>
  switch (url.path) {
  | [] => Home
  | ["edit"] => Edit(None)
  | ["edit", id] => Edit(Some(id))
  | ["login"] => Login
  | _ => NotFound
  };

let routeToString = (route: routes) =>
  switch (route) {
  | Home => "/"
  | Edit(None) => "/edit"
  | Edit(Some(id)) => "/edit/" ++ id
  | Login => "/login"
  | NotFound => "/404"
  };

module WithRouter = {
  type state = {currentRoute: routes};
  type action =
    | ChangeUrl(routes);
  let component = ReasonReact.reducerComponent("WithRouter");
  let make = children => {
    ...component,
    initialState: () => {
      currentRoute:
        ReasonReact.Router.dangerouslyGetInitialUrl() |> urlToRoute,
    },
    reducer: (action, _state) =>
      switch (action) {
      | ChangeUrl(route) => ReasonReact.Update({currentRoute: route})
      },
    didMount: self => {
      let watchId =
        ReasonReact.Router.watchUrl(url =>
          self.send(ChangeUrl(url |> urlToRoute))
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
      let href = routeToString(route);
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
        ...{(~currentRoute) => {
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
        }}
      </WithRouter>,
  };
};