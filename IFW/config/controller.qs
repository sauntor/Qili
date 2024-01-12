function Controller()
{
    console.log(`Controller @ ${new Date}`)
    installer.componentAdded.connect(Controller.prototype.onCompnentAdded.bind(this));
}
Controller.prototype.onCompnentAdded = function(added) {
    console.log(`CompnentAdded: ${added.displayName}`)
}
