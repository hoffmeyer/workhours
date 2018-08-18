open Types;

type state =
  | Initial
  | Starting
  | Stopping
  | Error(string);

type action =
  | StopWork
  | WorkStopped(work)
  | Failed(string)
  | StartWork
  | WorkStarted(work)
  | Reset;

let str = ReasonReact.string;

let unfinishedWork = workList : array(work) =>
  Js.Array.filter(w => w.duration == 0., workList);

let latestOnDate = (date, workList) =>
  workList
  |> Js.Array.filter(w => DateUtil.isSameDay(w.start, date))
  |> Js.Array.sortInPlaceWith((w1, w2) =>
       DateUtil.compareAsc(w1.start, w2.start)
     )
  |> Js.Array.pop;

let diffInHours = (d1, d2) =>
  (Js.Date.getTime(d2) -. Js.Date.getTime(d1)) /. 1000. /. 60. /. 60.;

let roundToQuarters = v => Js.Math.round(v *. 4.) /. 4.;

let dateToDiff = d =>
  diffInHours(d, Js.Date.make()) |> roundToQuarters |> string_of_float;

let inProgressWorkToday = l =>
  l |> unfinishedWork |> latestOnDate(Js.Date.make());

let component = ReasonReact.reducerComponent("Home");

let make =
    (~workList: array(work), ~handleAction: Types.action => unit, _children) => {
  ...component,
  initialState: _state => Initial,
  reducer: (action, _state) =>
    switch (action) {
    | StopWork => ReasonReact.Update(Stopping)
    | WorkStopped(work) =>
      handleAction(Types.WorkUpdate(work));
      ReasonReact.Update(Initial);
    | Failed(msg) => ReasonReact.Update(Error(msg))
    | StartWork =>
      ReasonReact.UpdateWithSideEffects(
        Starting,
        (
          self => {
            let newWork = {
              id: None,
              start: Js.Date.make(),
              duration: 0.,
              lunch: 0.,
              userid: None,
            };
            Js.Promise.(
              Fetch.fetchWithInit(
                "/api/work",
                Fetch.RequestInit.make(
                  ~method_=Post,
                  ~body=
                    Fetch.BodyInit.make(
                      newWork |> Encode.work |> Js.Json.stringify,
                    ),
                  ~headers=
                    Fetch.HeadersInit.make({
                      "Content-Type": "application/json",
                    }),
                  (),
                ),
              )
              |> then_(res => Fetch.Response.json(res))
              |> then_(json =>
                   json
                   |> Decode.work
                   |> (work => self.send(WorkStarted(work)))
                   |> resolve
                 )
              |> catch(err => {
                   Js.log2("Error starting work: ", err);
                   resolve(self.send(Failed("Error starting work")));
                 })
              |> ignore
            );
          }
        ),
      )
    | WorkStarted(work) =>
      handleAction(Types.WorkAdd(work));
      ReasonReact.Update(Initial);
    | Reset => ReasonReact.Update(Initial)
    },
  render: self =>
    <div>
      <h1> (str("Home page")) </h1>
      (
        switch (workList |> inProgressWorkToday) {
        | None =>
          <div>
            <p> (str("You are not working")) </p>
            <button onClick=(_evt => self.send(StartWork))>
              (str("Start work"))
            </button>
          </div>
        | Some(w) =>
          <div>
            <p> (str((w.start |> dateToDiff) ++ " hours and counting")) </p>
            <button onClick=(_evt => self.send(StopWork))>
              (str("Stop work"))
            </button>
          </div>
        }
      )
      (
        switch (self.state) {
        | Initial => <div />
        | Starting => <div> (str("Starting work...")) </div>
        | Stopping => <div> (str("Stopping work...")) </div>
        | Error(msg) =>
          <div>
            <p className="error"> (str("Error: " ++ msg)) </p>
            <button onClick=(_evt => self.send(Reset))> (str("Ok")) </button>
          </div>
        }
      )
    </div>,
};