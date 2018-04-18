open Belt;

type work = string;

type state =
  | Loading
  | Error
  | Loaded(array(string));

type action =
  | WorkFetch
  | WorkFetched(array(string))
  | WorkFetchFailed;

module Decode = {
    let work = json : array(work) =>
        Json.Decode.(
            json |> field("testData", array(string)) |> Array.map(_, work => work)
        );
};

let component = ReasonReact.reducerComponent("Work");

let make = (_children) => {
    ...component,
    initialState: _state => Loading,
    reducer: (action, _state) =>
    switch (action) {
        | WorkFetch =>
          ReasonReact.UpdateWithSideEffects(
            Loading,
            (
              self =>
                Js.Promise.(
                  Fetch.fetch("http://localhost:5000/api/test")
                  |> then_(Fetch.Response.json)
                  |> then_(json =>
                       json
                       |> Decode.work
                       |> (work => self.send(WorkFetched(work)))
                       |> resolve
                     )
                  |> catch(_err =>
                       Js.Promise.resolve(self.send(WorkFetchFailed))
                     )
                  |> ignore
                )
            ),
          )
        | WorkFetched(work) => ReasonReact.Update(Loaded(work))
        | WorkFetchFailed => ReasonReact.Update(Error)
    },
    didMount: self => {
        self.send(WorkFetch);
        ReasonReact.NoUpdate;
    },
    render: (self) =>
    switch (self.state) {
        | Error =>
          <div> (ReasonReact.stringToElement("An error occurred!")) </div>
        | Loading => <div> (ReasonReact.stringToElement("Loading...")) </div>
        | Loaded(dogs) =>
          <div>
            <h1> (ReasonReact.stringToElement("Dogs")) </h1>
            <p> (ReasonReact.stringToElement("Source: ")) </p>
            <a href="https://dog.ceo">
              (ReasonReact.stringToElement("https://dog.ceo"))
            </a>
            <ul>
              (
                Array.map(dogs, dog =>
                  <li key=dog> (ReasonReact.stringToElement(dog)) </li>
                )
                |> ReasonReact.arrayToElement
              )
            </ul>
          </div>
    },
};