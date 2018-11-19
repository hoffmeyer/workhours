import {Router} from 'express';
import {authenticate} from 'passport';

const router = Router();

router.get('/', (req, res) => {
  console.log('showing log in');
  res.render('login');
});

router.post(
  '/',
  authenticate('local', {
    successRedirect: '/',
    failureRedirect: '/login',
    failureFlash: true,
  }),
);

export default router;
