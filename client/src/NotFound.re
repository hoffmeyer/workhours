let component = ReasonReact.statelessComponent("NotFound");

let make = _children => {
  ...component,
  render: _self => <h1> {"Whoops page not found" |> ReasonReact.string} </h1>,
};