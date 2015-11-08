//
//  ResultsViewController.swift
//  AutoTester
//
//  Created by jmnavarro on 13/10/15.
//  Copyright © 2015 mousebird consulting. All rights reserved.
//

import UIKit

class ResultsViewController: UITableViewController {

	var titles = [String]()
	var results = [MaplyTestResult]()

	let queue = { () -> NSOperationQueue in
		let queue = NSOperationQueue()

		queue.maxConcurrentOperationCount = 1
		queue.qualityOfService = .Utility

		return queue
	}()

	override func tableView(tableView: UITableView,
		numberOfRowsInSection section: Int) -> Int {

		return results.count
	}

	override func tableView(tableView: UITableView,
		cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {

		let cell = tableView.dequeueReusableCellWithIdentifier("result", forIndexPath: indexPath) as! ResultCell

		let title = titles[indexPath.row]
		cell.nameLabel?.text = title

		queue.addOperationWithBlock {
			let baselineImage = UIImage(contentsOfFile: self.results[indexPath.row].baselineImageFile)

			let actualImage: UIImage?

			if let actualImageFile = self.results[indexPath.row].actualImageFile {
				actualImage = UIImage(contentsOfFile: actualImageFile)
			}
			else {
				actualImage = nil
			}

			dispatch_async(dispatch_get_main_queue()) {
				cell.baselineImage?.image = baselineImage
				cell.actualImage?.image = actualImage
			}
		}

		return cell
	}

	override func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath) {

		let cell = tableView.cellForRowAtIndexPath(indexPath)

		self.performSegueWithIdentifier("showFullScreen", sender: cell)
	}

	override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
		let destination = segue.destinationViewController as! FullScreenViewController

		if let cell = sender as? ResultCell {
			destination.title = cell.nameLabel?.text
			destination.actualImageResult = cell.actualImage?.image
			destination.baselineImageResult = cell.baselineImage?.image
		}
	}

}
