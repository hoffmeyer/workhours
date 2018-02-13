let component = ReasonReact.statelessComponent("Home");

let make = (_children) => {
    ...component,
    render: (_self) =>
        <div>
            <h1>(ReasonReact.stringToElement("Home page"))</h1>
        </div>
};