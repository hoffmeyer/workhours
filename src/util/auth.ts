const isLoggedIn = (req, res, next) => {
  // if user is authenticated in the session, carry on
  if (req.isAuthenticated()) return next();

  // if they aren't redirect them to the login page
  res.redirect('/login');
};

const isLoggedInApi = (req, res, next) => {
  // if user is authenticated in the session, carry on
  if (req.isAuthenticated()) return next();

  res.statusCode = 401;
  res.send('Not authenticated');
};

export {
  isLoggedIn,
  isLoggedInApi
};