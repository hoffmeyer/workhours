let component = ReasonReact.statelessComponent("Login");

let make = (_children) => {
    ...component,
    render: (_self) =>
        <div>
            <h1>(ReasonReact.stringToElement("Login page"))</h1>
        </div>
};