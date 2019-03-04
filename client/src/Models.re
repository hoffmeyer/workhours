open Types;

let doFetch = (method, body) =>
  Fetch.RequestInit.make(
    ~method_=method,
    ~body=Fetch.BodyInit.make(body),
    ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
    (),
  );

let post = doFetch(Post);
let delete = doFetch(Delete);

let authenticate = res => {
  let status = Fetch.Response.status(res);
  switch (status) {
  | 200 => Fetch.Response.json(res)
  | 401 =>
    ReasonReact.Router.push("/login");
    Js.Exn.raiseError(Fetch.Response.statusText(res));
  | _ => Js.Exn.raiseError(Fetch.Response.statusText(res))
  };
};

module Work = {
  let save = work =>
    Js.Promise.(
      Fetch.fetchWithInit(
        "/api/work",
        post(work |> Encode.work |> Js.Json.stringify),
      )
      |> then_(authenticate)
      |> then_(json => json |> Decode.work |> resolve)
    );

  let delete = (id: string) =>
    Js.Promise.(
      Fetch.fetchWithInit("/api/work", delete("{\"id\": \"" ++ id ++ "\"}"))
      |> then_(authenticate)
      |> then_(_t => resolve())
    );

  let getLatest =
    Js.Promise.(
      Fetch.fetch("/api/work/latest")
      |> then_(authenticate)
      |> then_(json => json |> Decode.workList |> resolve)
    );

  let balance =
    Js.Promise.(
      Fetch.fetch("/api/work/balance")
      |> then_(authenticate)
      |> then_(json => json |> Decode.balance |> resolve)
    );
};

module User = {
  let save = user =>
    Js.Promise.(
      Fetch.fetchWithInit(
        "/api/user",
        post(user |> Encode.user |> Js.Json.stringify),
      )
      |> then_(authenticate)
      |> then_(_ => resolve())
    );
};