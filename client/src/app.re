[%bs.raw {|require('./app.css')|}];

[@bs.module] external logo : string = "./logo.svg";

type action =
  | ShowHome
  | ShowWork;

type page =
  | Home
  | Work;

type state = {currentPage: page };

let component = ReasonReact.reducerComponent("App");

let make = (~text, _children) => {
  ...component,
  initialState: () => {currentPage: Home },
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
            switch (url.path){
            | ["work"] => self.send(ShowWork)
            | _ => self.send(ShowHome) 
            }
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
