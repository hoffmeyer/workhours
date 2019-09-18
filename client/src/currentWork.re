open Types;

type state =
  | Initial
  | Working((Js.Date.t, Js.Date.t))
  | Starting
  | Stopping
  | Error(string);

type action =
  | StopWork
  | WorkStopped(work)
  | Failed(string)
  | StartWork
  | WorkStarted(work)
  | Reset
  | Tick;

let str = ReasonReact.string;

let unfinishedWork = (workList) =>
  Js.Array.filter((w: work) => w.duration == 0., workList);

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

let dateToDiff = d => diffInHours(d, Js.Date.make()) |> roundToQuarters;

let inProgressWorkToday = l =>
  l |> unfinishedWork |> latestOnDate(Js.Date.make());

let workInProgressToUpdatedWork = (inProgress: work, hours) => {
  id: inProgress.id,
  start: inProgress.start,
  duration: hours,
  lunch: hours > 4. ? 0.5 : 0.,
  userid: inProgress.userid,
};

let diffInHours = start => {
  let now = Js.Date.make();
  let hours =
    (Js.Date.getTime(now) -. Js.Date.getTime(start)) /. (60. *. 60. *. 1000.);
  (hours *. 4. |> Js.Math.round) /. 4.;
};

let stopWork = workList =>
  ReasonReact.UpdateWithSideEffects(
    Stopping,
    self =>
      switch (workList |> inProgressWorkToday) {
      | None => self.send(Failed("Trying to stop work not in progress"))
      | Some(inProgress) =>
        let duration = diffInHours(inProgress.start);
        Js.Promise.(
          {
            Models.Work.save({
              ...inProgress,
              duration,
              lunch:
                inProgress.lunch == 0. && duration > 4.
                  ? 0.5 : inProgress.lunch,
            })
            |> then_(work => self.send(WorkStopped(work)) |> resolve)
            |> catch(err => {
                 Js.log2("Error starting work: ", err);
                 resolve(self.send(Failed("Error starting work")));
               })
            |> ignore;
          }
        );
      },
  );

let startWork =
  ReasonReact.UpdateWithSideEffects(
    Starting,
    self => {
      let newWork = {
        id: None,
        start: Js.Date.make(),
        duration: 0.,
        lunch: 0.,
        userid: None,
      };
      Js.Promise.(
        Models.Work.save(newWork)
        |> then_(work => self.send(WorkStarted(work)) |> resolve)
        |> catch(err => {
             Js.log2("Error starting work: ", err);
             resolve(self.send(Failed("Error starting work")));
           })
        |> ignore
      );
    },
  );

let component = ReasonReact.reducerComponent("Home");

let make =
    (~workList: array(work), ~handleAction: Types.action => unit, _children) => {
  ...component,
  initialState: () =>
    switch (workList |> inProgressWorkToday) {
    | None => Initial
    | Some(w) => Working((w.start, Js.Date.make()))
    },
  didMount: self => {
    let interval = Js.Global.setInterval(() => self.send(Tick), 1000);
    self.onUnmount(() => Js.Global.clearInterval(interval));
  },
  reducer: (action, _state) =>
    switch (action) {
    | StopWork => stopWork(workList)
    | WorkStopped(work) =>
      handleAction(Types.WorkUpdate(work));
      ReasonReact.Update(Initial);
    | Failed(msg) => ReasonReact.Update(Error(msg))
    | StartWork => startWork
    | WorkStarted(work) =>
      handleAction(Types.WorkAdd(work));
      ReasonReact.Update(Initial);
    | Reset => ReasonReact.Update(Initial)
    | Tick =>
      ReasonReact.Update(
        switch (workList |> inProgressWorkToday) {
        | None => Initial
        | Some(w) => Working((w.start, Js.Date.make()))
        },
      )
    },
  render: self =>
    <div className="mb-l">
      {switch (self.state) {
       | Initial =>
         <div>
           <h1> {str("You are not working")} </h1>
           <button id="startStop" onClick={_evt => self.send(StartWork)}>
             {str("Start working")}
           </button>
         </div>
       | Working((start, _)) =>
         <div>
           <h1>
             {str(
                (start |> dateToDiff |> Printf.sprintf("%.2f"))
                ++ " hours and counting"
              )}
           </h1>
           <button id="startStop" onClick={_evt => self.send(StopWork)}>
             {str("Stop working")}
           </button>
         </div>
       | Starting => <div> {str("Starting work...")} </div>
       | Stopping => <div> {str("Stopping work...")} </div>
       | Error(msg) =>
         <div>
           <p className="error"> {str("Error: " ++ msg)} </p>
           <button onClick={_evt => self.send(Reset)}> {str("Ok")} </button>
         </div>
       }}
    </div>,
};