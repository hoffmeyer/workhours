open Types;

let routeToComponent =
    (
      route: Router.routes,
      ~workList: array(work),
      ~handleAction: action => unit,
    ) =>
  switch (route) {
  | Home => <Home workList handleAction />
  | Edit(id) => <NewWork workList id handleAction />
  | List => <WorkList workList handleAction />
  | Login => <Login />
  };