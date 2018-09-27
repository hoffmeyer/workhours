let component = ReasonReact.statelessComponent("NewWork");

let make = _children => {
  ...component,
  render: _self =>
    <div> <h1> (ReasonReact.string("New work page")) </h1> </div>,
};