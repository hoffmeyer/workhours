let component = ReasonReact.statelessComponent("NewWork");

let make = _children => {
  ...component,
  render: _self =>
    <div> <h1> (ReasonReact.stringToElement("New work page")) </h1> </div>,
};