import React from "react";
import logo from "./logo.svg";
import "./App.scss";
import { Provider } from "react-redux";
import store from "./redux/store";
import {BrowserRouter as Router, Switch, Route, Link } from 'react-router-dom';
import Home from "./pages/Home";
import Login from "./pages/Login";
import Settings from "./pages/Settings";

function App() {
  return (
    <Provider store={store}>
      <Router>
      <div className="App">
        <div className="App-header">
          <ul>
            <li>
              <Link to="/">Home</Link>
            </li>
            <li>
              <Link to="/Login">Login</Link>
            </li>
            <li>
              <Link to="/Settings">Settings</Link>
            </li>
          </ul>
        </div>
        <main>
          <Switch>
            <Route path="/Login">
              <Login/>
            </Route>
            <Route path="/Settings">
              <Settings/>
            </Route>
            <Route path="/">
              <Home/>
            </Route>
          </Switch>
        </main>
      </div>
      </Router>
    </Provider>
  );
}

export default App;
