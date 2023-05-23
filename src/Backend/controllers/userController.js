const userget = async (req, res) => {
    try {
      console.log(req.body);
      const user = new User(req.body);
      await user.save();
      res.send();
    } catch (error) {
      console.log(error);
      res.status(500).send();
    }
  }