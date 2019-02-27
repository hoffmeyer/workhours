import { Router } from 'express';
import * as R from 'ramda';
import user from '../../models/user';
import { isLoggedInApi } from '../../util/auth';

let router = Router();

router.get('/', isLoggedInApi, (req, res) => {
    const id: string = req.user.id;
    user.byId(id).then(user => res.json(user));
});

export default router;