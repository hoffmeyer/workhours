[%bs.raw {|require('./app.css')|}];

[@bs.module] external logo : string = "./logo.svg";

type action =
  | ShowHome
  | ShowWork;

type page =
  | Home
  | Work;

type state = {currentPage: page };

let pathToAction = (path) => {
  switch (path){
    | ["work"] => ShowWork
    | _ => ShowHome 
  }
};

let actionToPage = (action) => {
  switch (action){
  | ShowHome => Home
  | ShowWork => Work
  }
};

let component = ReasonReact.reducerComponent("App");

let make = (~text, _children) => {
  ...component,
  initialState: () => {
    {
      currentPage: actionToPage(pathToAction(ReasonReact.Router.dangerouslyGetInitialUrl().path))
    }
  },
  reducer: (action, _state) => {
    switch (action) {
    | ShowHome => ReasonReact.Update({currentPage: Home})
    | ShowWork => ReasonReact.Update({currentPage: Work})
    }
  },
  subscriptions: (self) => [
    Sub(
      () =>
        ReasonReact.Router.watchUrl(
          url => {
            self.send(pathToAction(url.path))
          }
        ),
      ReasonReact.Router.unwatchUrl
    )
  ],
  render: (self) =>
    <div className="App">
      <div className="App-header">
        <img src=logo className="App-logo" alt="logo" />
        <h2> (ReasonReact.stringToElement(text)) </h2>
        <Link href="/">(ReasonReact.stringToElement("Home"))</Link>
        <Link href="/work">(ReasonReact.stringToElement("Work"))</Link>
      </div>
      (switch (self.state.currentPage){
      | Work => <Work/>
      | _ => <Home/>
      })
    </div>
};
